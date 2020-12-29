from django_filters import rest_framework as filters
from rest_framework import viewsets
from rest_framework.permissions import AllowAny
from rest_framework import filters as rest_filters
from rest_framework.response import Response

from chat_app_service.serializer.message import MessageSerializer
from chat_app_service.models import Message, Room


class MessageViewSet(viewsets.ModelViewSet):
    queryset = Message.objects.all()
    permission_classes = (AllowAny,)
    serializer_class = MessageSerializer
    filter_backends = (filters.DjangoFilterBackend, rest_filters.SearchFilter, rest_filters.OrderingFilter)

    # Message.objects.filter(room_id=).order)

    def list(self, request, *args, **kwargs):
        queryset = self.filter_queryset(self.get_queryset())
        room_id = request.query_params.get('room_id')
        from_user = request.query_params.get('from_user')
        if room_id:
            queryset = Message.objects.filter(room_id=room_id)
        if from_user:
            queryset = Message.objects.filter(from_user=from_user)

        page = self.paginate_queryset(queryset)
        if page is not None:
            serializer = self.get_serializer(page, many=True)
            return self.get_paginated_response(serializer.data)

        serializer = self.get_serializer(queryset, many=True)
        return Response(serializer.data)
