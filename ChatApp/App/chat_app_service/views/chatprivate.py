from collections import OrderedDict
from operator import itemgetter

from django_filters import rest_framework as filters
from rest_framework import viewsets, status
from rest_framework.permissions import AllowAny
from rest_framework import filters as rest_filters
from rest_framework.response import Response

from chat_app_service.serializer.chatprivate import ChatPrivateSerializer
from chat_app_service.models import ChatPrivate, User


class ChatPrivateFilter(filters.FilterSet):
    from_user = filters.NumberFilter(field_name='from_user')
    to_user = filters.NumberFilter(field_name='to_user')

    class Meta:
        model = ChatPrivate
        fields = ['from_user', 'to_user']


class ChatPrivateViewSet(viewsets.ModelViewSet):
    queryset = ChatPrivate.objects.all().order_by('-created_at')
    permission_classes = (AllowAny,)
    serializer_class = ChatPrivateSerializer
    filter_backends = (filters.DjangoFilterBackend, rest_filters.SearchFilter, rest_filters.OrderingFilter)

    # filter_class = ChatPrivateFilter
    def list(self, request, *args, **kwargs):
        queryset = self.filter_queryset(self.get_queryset())
        from_user = request.query_params.get('from_user')
        to_user = request.query_params.get('to_user')
        data = []
        if from_user is not None:
            queryset = ChatPrivate.objects.filter(from_user=from_user)
            serializer = self.get_serializer(queryset, many=True)
            for user in serializer.data:
                user_dict = dict(OrderedDict(user))
                if to_user == str(user_dict['to_user']) and user_dict not in data:
                    data.append(user_dict)

        if to_user is not None:
            queryset = ChatPrivate.objects.filter(from_user=to_user)
            serializer = self.get_serializer(queryset, many=True)
            for user_ in serializer.data:
                user_dict_ = dict(OrderedDict(user_))
                if from_user == str(user_dict_['to_user']) and user_dict_ not in data:
                    data.append(user_dict_)
        if len(data) > 0:
            sort_dict = sorted(data, key=itemgetter('created_at'))
            return Response(sort_dict)
        else:
            serializer = self.get_serializer(queryset, many=True)
            return Response(serializer.data)
