import django_filters
from django.http import QueryDict
from django_filters import rest_framework as filters
from rest_framework import viewsets
from rest_framework.permissions import AllowAny
from rest_framework import filters as rest_filters
from django_filters import rest_framework as filters
from rest_framework.response import Response

from chat_app_service.serializer.users import UserSerializer
from chat_app_service.models import User


class UserFilter(django_filters.FilterSet):
    class Meta:
        model = User
        fields = ['username']


class UserViewSet(viewsets.ModelViewSet):
    queryset = User.objects.all().order_by('-created_at')
    permission_classes = (AllowAny,)
    serializer_class = UserSerializer
    filter_backends = (filters.DjangoFilterBackend, rest_filters.SearchFilter, rest_filters.OrderingFilter)
    filter_class = UserFilter


